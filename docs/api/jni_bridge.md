# JNI Bridge API Reference

The Java Native Interface (JNI) Bridge allows the Android Kotlin/Java layer to communicate with the high-performance C++ core. This document lists the available native classes and methods.

## Namespace: `com.minni.framework.core`

### 1. KnowledgeGraph

In-memory graph database for symbolic reasoning.

| Java Method | Native C++ Implementation | Description |
|-------------|---------------------------|-------------|
| `nativeInit(boolean useQuantization)` | `KnowledgeGraph::KnowledgeGraph` | Initialize graph. `useQuantization=true` enables Int8 storage. |
| `nativeAddFact(String sub, String pred, String obj)` | `KnowledgeGraph::add_fact` | Add a triple (Subject, Predicate, Object). |
| `nativeGetNumFacts()` | `KnowledgeGraph::num_facts` | Get total number of facts. |
| `nativeSave(String path, String key)` | `KnowledgeGraph::save` | Save to binary file (encrypted if key provided). |
| `nativeLoad(String path, String key)` | `KnowledgeGraph::load` | Load from binary file. |

### 2. VectorStore

Vector database for similarity search (Embeddings).

| Java Method | Native C++ Implementation | Description |
|-------------|---------------------------|-------------|
| `nativeInit(boolean useQuantization)` | `VectorStore::VectorStore` | Initialize store. `useQuantization=true` enables Int8 storage. |
| `nativeAddVector(String id, float[] vec)` | `VectorStore::add_vector` | Add a vector with an ID. |
| `nativeSearch(float[] query, int limit)` | `VectorStore::search` | Find nearest neighbors using Cosine Similarity. |
| `nativeSave(String path, String key)` | `VectorStore::save` | Save to binary file (encrypted if key provided). |
| `nativeLoad(String path, String key)` | `VectorStore::load` | Load from binary file. |

### 3. SatSolver

Boolean Satisfiability (SAT) Solver.

| Java Method | Native C++ Implementation | Description |
|-------------|---------------------------|-------------|
| `checkNative(String formula)` | `SatSolver::check` | Check if a boolean formula is satisfiable. |
| `getModelNative()` | `SatSolver::get_model` | Get the variable assignment that satisfies the formula. |

### 4. RuleEngine

Rule-based inference engine.

| Java Method | Native C++ Implementation | Description |
|-------------|---------------------------|-------------|
| `nativeLoadRules(String rules)` | `RuleEngine::load_rules` | Parse rules from a string. |
| `nativeEvaluate(Map<String, Object> inputs)` | `RuleEngine::evaluate` | Evaluate rules against input variables. |

### 5. DSPKernel & SignalProcessor

Digital Signal Processing utilities.

| Java Method | Native C++ Implementation | Description |
|-------------|---------------------------|-------------|
| `fft(float[] r, float[] i, boolean inv)` | `DSPKernel::fft` | In-place Fast Fourier Transform. |
| `complexMagnitude(r, i, out)` | `DSPKernel::complex_magnitude` | Calculate magnitude of complex spectrum. |
| `SignalProcessor::load(float[] data)` | `SignalProcessor::load` | Load audio/sensor data into processing chain. |
| `SignalProcessor::applyWindow(float[] win)` | `SignalProcessor::apply_window` | Apply window function. |
| `SignalProcessor::fft(boolean inv)` | `SignalProcessor::fft` | Run FFT on loaded data. |
| `SignalProcessor::getOutput()` | `SignalProcessor::get_output` | Get result from chain. |

---

## Namespace: `com.minni.framework.genai`

### 1. ModelRunner

Wrapper for LiteRT (TensorFlow Lite).

| Java Method | Native C++ Implementation | Description |
|-------------|---------------------------|-------------|
| `nativeLoadModel(String path, int threads)` | `ModelRunner::load_model` | Load `.tflite` model from file system. |
| `nativePredict(Tensor[] inputs)` | `ModelRunner::predict` | Run inference and return output tensors. |

**Tensor Object:**
Maps C++ `minni::genai::Tensor` to `com.minni.framework.genai.Tensor`.
- `name` (String)
- `shape` (int[])
- `data` (float[])

## Memory Management

All native objects (`nativeHandle`) must be explicitly freed. The Java wrappers implement `Closeable` (or `AutoCloseable`). Always call `close()` or use `use { }` blocks in Kotlin to prevent native memory leaks.

```kotlin
// Example
val store = VectorStore()
try {
    // use store...
} finally {
    store.close() // Calls nativeFree()
}
```

#ifndef MINNI_CORE_GENAI_MODEL_RUNNER_H_
#define MINNI_CORE_GENAI_MODEL_RUNNER_H_

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

namespace minni {
namespace genai {

// Simple tensor abstraction
struct Tensor {
    std::vector<int> shape;
    std::vector<float> data; // Assumes float32 for simplicity for now
    std::string name;
};

/**
 * Wrapper around TensorFlow Lite (LiteRT) Interpreter.
 * Loads models and runs inference.
 */
class ModelRunner {
public:
    ModelRunner();
    ~ModelRunner();

    /**
     * Load a TFLite model from a file path.
     * @param model_path Path to the .tflite file.
     * @param num_threads Number of threads to use for inference.
     * @return true if successful.
     */
    bool load_model(const std::string& model_path, int num_threads = 4);

    /**
     * Run inference.
     * @param inputs List of input tensors (must match model input signature).
     * @return List of output tensors.
     */
    std::vector<Tensor> predict(const std::vector<Tensor>& inputs);

    /**
     * Get input tensor count.
     */
    int get_input_count() const;

    /**
     * Get output tensor count.
     */
    int get_output_count() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace genai
} // namespace minni

#endif // MINNI_CORE_GENAI_MODEL_RUNNER_H_

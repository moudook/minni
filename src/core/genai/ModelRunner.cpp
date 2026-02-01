#include "ModelRunner.h"
#include <iostream>
#include <fstream>
#include <cstring>

// Check if we have TFLite available (Mocking for this environment)
#ifdef MINNI_USE_TFLITE
    #include "tensorflow/lite/interpreter.h"
    #include "tensorflow/lite/kernels/register.h"
    #include "tensorflow/lite/model.h"
    #include "tensorflow/lite/tools/gen_op_registration.h"
#else
    // Mock classes to allow compilation without TFLite installed
    namespace tflite {
        namespace ops {
            namespace builtin {
                class BuiltinOpResolver {};
            }
        }

        class FlatBufferModel {
        public:
            static std::unique_ptr<FlatBufferModel> BuildFromFile(const char* filename) {
                // Check if file exists
                std::ifstream f(filename);
                if (f.good()) return std::unique_ptr<FlatBufferModel>(new FlatBufferModel());
                return nullptr;
            }
        };

        class Interpreter {
        public:
            void SetNumThreads(int) {}
            int AllocateTensors() { return 0; } // kTfLiteOk
            int Invoke() { return 0; }

            // Mock indices
            std::vector<int> inputs() { return {0}; }
            std::vector<int> outputs() { return {0}; }

            // Mock Tensor access
            template<typename T>
            T* typed_input_tensor(int i) {
                return reinterpret_cast<T*>(mock_buffer);
            }

            template<typename T>
            T* typed_output_tensor(int i) {
                return reinterpret_cast<T*>(mock_buffer);
            }

            // Mock Tensor info
            struct TfLiteTensor {
                size_t bytes;
                int type;
            };

            TfLiteTensor* tensor(int i) {
                static TfLiteTensor t = {4096 * 4, 1}; // Enough for small tests
                return &t;
            }

        private:
            char mock_buffer[4096 * 4]; // Dummy buffer
        };

        class InterpreterBuilder {
        public:
            InterpreterBuilder(const FlatBufferModel&, const ops::builtin::BuiltinOpResolver&) {}
            InterpreterBuilder(const FlatBufferModel&, const void*) {}

            int operator()(std::unique_ptr<Interpreter>* interpreter) {
                *interpreter = std::unique_ptr<Interpreter>(new Interpreter());
                return 0;
            }
        };
    }
#endif

namespace minni {
namespace genai {

struct ModelRunner::Impl {
    std::unique_ptr<tflite::FlatBufferModel> model;
    std::unique_ptr<tflite::Interpreter> interpreter;
    tflite::ops::builtin::BuiltinOpResolver resolver;
};

ModelRunner::ModelRunner() : impl_(std::make_unique<Impl>()) {}

ModelRunner::~ModelRunner() = default;

bool ModelRunner::load_model(const std::string& model_path, int num_threads) {
    impl_->model = tflite::FlatBufferModel::BuildFromFile(model_path.c_str());
    if (!impl_->model) {
        std::cerr << "Failed to load model: " << model_path << std::endl;
        return false;
    }

    tflite::InterpreterBuilder builder(*impl_->model, impl_->resolver);
    if (builder(&impl_->interpreter) != 0) { // kTfLiteOk is usually 0
        std::cerr << "Failed to build interpreter" << std::endl;
        return false;
    }

    impl_->interpreter->SetNumThreads(num_threads);

    if (impl_->interpreter->AllocateTensors() != 0) {
        std::cerr << "Failed to allocate tensors" << std::endl;
        return false;
    }

    return true;
}

std::vector<Tensor> ModelRunner::predict(const std::vector<Tensor>& inputs) {
    std::vector<Tensor> outputs;
    if (!impl_->interpreter) return outputs;

    // Check input count
#ifdef MINNI_USE_TFLITE
    const auto& input_indices = impl_->interpreter->inputs();
    if (inputs.size() != input_indices.size()) {
         std::cerr << "Input mismatch" << std::endl;
         return outputs;
    }

    for (size_t i = 0; i < inputs.size(); ++i) {
        int tensor_idx = input_indices[i];
        // Assuming float32 for now
        float* input_data = impl_->interpreter->typed_input_tensor<float>(i);
        if (!input_data) continue;

        // Safety copy
        size_t bytes = impl_->interpreter->tensor(tensor_idx)->bytes;
        // size_t count = bytes / sizeof(float);

        if (inputs[i].data.size() * sizeof(float) > bytes) {
            // Error buffer too small
             std::cerr << "Input buffer too small for input " << i << std::endl;
             continue;
        }

        std::memcpy(input_data, inputs[i].data.data(), inputs[i].data.size() * sizeof(float));
    }
#else
    // Mock behavior: Copy inputs if needed, or just proceed
#endif

    // Run Inference
    if (impl_->interpreter->Invoke() != 0) {
        std::cerr << "Inference failed" << std::endl;
        return outputs;
    }

    // Retrieve outputs
#ifdef MINNI_USE_TFLITE
    const auto& output_indices = impl_->interpreter->outputs();
    outputs.resize(output_indices.size());

    for (size_t i = 0; i < output_indices.size(); ++i) {
        int tensor_idx = output_indices[i];
        float* output_data = impl_->interpreter->typed_output_tensor<float>(i);

        if (output_data) {
             size_t bytes = impl_->interpreter->tensor(tensor_idx)->bytes;
             size_t count = bytes / sizeof(float);

             outputs[i].data.assign(output_data, output_data + count);
             // Shape logic would go here (retrieving dims from tensor)
        }
    }
#else
    // Mock behavior: return a dummy output
    Tensor dummy;
    dummy.data = {0.1f, 0.9f}; // Dummy probabilities
    dummy.shape = {1, 2};
    outputs.push_back(dummy);
#endif

    return outputs;
}

int ModelRunner::get_input_count() const {
    if (!impl_->interpreter) return 0;
    return impl_->interpreter->inputs().size();
}

int ModelRunner::get_output_count() const {
    if (!impl_->interpreter) return 0;
    return impl_->interpreter->outputs().size();
}

} // namespace genai
} // namespace minni

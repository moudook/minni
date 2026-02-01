#include "genai/ModelRunner.h"
#include <iostream>
#include <vector>
#include <cassert>

using namespace minni::genai;

void test_model_runner_lifecycle() {
    std::cout << "Testing ModelRunner Lifecycle..." << std::endl;
    ModelRunner runner;
    // Load a non-existent file - mock should fail or return false if we strictly check file existence in mock
    // In my mock impl: FlatBufferModel::BuildFromFile checks std::ifstream(filename).good()
    // So let's create a dummy file first

    std::string dummy_model = "dummy_model.tflite";
    // Create empty file
    FILE* f = fopen(dummy_model.c_str(), "w");
    if (f) fclose(f);

    bool loaded = runner.load_model(dummy_model, 2);
    // With my mock implementation, this should succeed if file exists
    if (loaded) {
        std::cout << "  Model loaded successfully (Mock)." << std::endl;
    } else {
        std::cout << "  Model failed to load." << std::endl;
    }

    // Cleanup
    remove(dummy_model.c_str());
}

void test_inference() {
    std::cout << "Testing ModelRunner Inference (Mock)..." << std::endl;
    ModelRunner runner;

    // Setup dummy file for load
    std::string dummy_model = "dummy_model.tflite";
    FILE* f = fopen(dummy_model.c_str(), "w");
    if (f) fclose(f);
    runner.load_model(dummy_model);
    remove(dummy_model.c_str());

    // Create Input Tensor
    Tensor input;
    input.name = "input_0";
    input.shape = {1, 224, 224, 3};
    input.data.resize(1 * 224 * 224 * 3, 0.5f); // Fill with dummy data

    std::vector<Tensor> inputs = {input};

    // Run Predict
    std::vector<Tensor> outputs = runner.predict(inputs);

    // Mock implementation returns 1 output tensor
    assert(outputs.size() > 0);
    std::cout << "  Output received. Shape: [";
    for(int d : outputs[0].shape) std::cout << d << " ";
    std::cout << "]" << std::endl;

    assert(outputs[0].data.size() > 0);

    std::cout << "  Inference test passed." << std::endl;
}

int main() {
    test_model_runner_lifecycle();
    test_inference();
    std::cout << "All ModelRunner tests passed!" << std::endl;
    return 0;
}

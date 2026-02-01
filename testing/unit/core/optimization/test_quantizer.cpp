#include "../../../../src/core/optimization/Quantizer.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>

void test_quantization() {
    std::cout << "Running Quantizer Test..." << std::endl;

    // Test data: Range [-1.0, 1.0]
    std::vector<float> data = {-1.0f, -0.5f, 0.0f, 0.5f, 1.0f};

    // 1. Calculate Params
    auto params = minni::optimization::Quantizer::calculate_params(data);

    std::cout << "Scale: " << params.scale << ", Zero Point: " << params.zero_point << std::endl;

    // 2. Quantize
    auto quantized = minni::optimization::Quantizer::quantize(data, params);

    assert(quantized.size() == data.size());

    // 3. Dequantize
    auto dequantized = minni::optimization::Quantizer::dequantize(quantized, params);

    // 4. Verify Error is within bounds
    // Max error for linear quantization is scale / 2
    float max_error = params.scale / 2.0f + 1e-5f; // Small epsilon

    for (size_t i = 0; i < data.size(); ++i) {
        float diff = std::abs(data[i] - dequantized[i]);
        // std::cout << "Orig: " << data[i] << " -> Q: " << (int)quantized[i]
        //           << " -> DeQ: " << dequantized[i] << " (Diff: " << diff << ")" << std::endl;
        assert(diff <= max_error);
    }

    // Test extreme values (clamping)
    float val = 1000.0f; // Way outside original range
    int8_t q = minni::optimization::Quantizer::quantize_scalar(val, params);
    assert(q == 127); // Should clamp to max int8

    float val_neg = -1000.0f;
    int8_t q_neg = minni::optimization::Quantizer::quantize_scalar(val_neg, params);
    assert(q_neg == -128); // Should clamp to min int8

    std::cout << "Quantizer Test Passed!" << std::endl;
}

int main() {
    test_quantization();
    return 0;
}

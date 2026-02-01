#include "Quantizer.h"
#include <algorithm>
#include <cmath>
#include <limits>

namespace minni {
namespace optimization {

Quantizer::QuantizationParams Quantizer::calculate_params(const std::vector<float>& data) {
    if (data.empty()) {
        return {1.0f, 0};
    }

    float min_val = data[0];
    float max_val = data[0];

    for (float v : data) {
        if (v < min_val) min_val = v;
        if (v > max_val) max_val = v;
    }

    // Ensure range is not zero
    if (std::abs(max_val - min_val) < 1e-6) {
        return {1.0f, 0};
    }

    // Int8 range: [-128, 127] -> 255 steps
    const int32_t qmin = -128;
    const int32_t qmax = 127;

    float scale = (max_val - min_val) / (qmax - qmin);
    int32_t zero_point = static_cast<int32_t>(std::round(qmin - min_val / scale));

    // Clamp zero_point to valid range (though it should be naturally)
    if (zero_point < -128) zero_point = -128;
    if (zero_point > 127) zero_point = 127;

    return {scale, zero_point};
}

std::vector<int8_t> Quantizer::quantize(const std::vector<float>& data, const QuantizationParams& params) {
    std::vector<int8_t> quantized;
    quantized.reserve(data.size());

    for (float v : data) {
        quantized.push_back(quantize_scalar(v, params));
    }
    return quantized;
}

std::vector<float> Quantizer::dequantize(const std::vector<int8_t>& data, const QuantizationParams& params) {
    std::vector<float> dequantized;
    dequantized.reserve(data.size());

    for (int8_t q : data) {
        dequantized.push_back(dequantize_scalar(q, params));
    }
    return dequantized;
}

int8_t Quantizer::quantize_scalar(float value, const QuantizationParams& params) {
    // q = round(val / scale + zero_point)
    int32_t q = static_cast<int32_t>(std::round(value / params.scale) + params.zero_point);

    // Clamp
    if (q < -128) return -128;
    if (q > 127) return 127;
    return static_cast<int8_t>(q);
}

float Quantizer::dequantize_scalar(int8_t value, const QuantizationParams& params) {
    // val = (q - zero_point) * scale
    return (static_cast<int32_t>(value) - params.zero_point) * params.scale;
}

} // namespace optimization
} // namespace minni

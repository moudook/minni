#ifndef MINNI_CORE_OPTIMIZATION_QUANTIZER_H_
#define MINNI_CORE_OPTIMIZATION_QUANTIZER_H_

#include <vector>
#include <cstdint>
#include <utility>

namespace minni {
namespace optimization {

/**
 * Utility class for Quantization (Float32 -> Int8).
 * Supports both symmetric and asymmetric quantization.
 */
class Quantizer {
public:
    struct QuantizationParams {
        float scale;
        int32_t zero_point;
    };

    /**
     * Calculate quantization parameters for a given float vector.
     * Maps [min, max] to [-128, 127] (int8).
     */
    static QuantizationParams calculate_params(const std::vector<float>& data);

    /**
     * Quantize a vector of floats to int8.
     */
    static std::vector<int8_t> quantize(const std::vector<float>& data, const QuantizationParams& params);

    /**
     * Dequantize a vector of int8 back to float.
     * value = (q - zero_point) * scale
     */
    static std::vector<float> dequantize(const std::vector<int8_t>& data, const QuantizationParams& params);

    /**
     * Quantize a single value.
     */
    static int8_t quantize_scalar(float value, const QuantizationParams& params);

    /**
     * Dequantize a single value.
     */
    static float dequantize_scalar(int8_t value, const QuantizationParams& params);
};

} // namespace optimization
} // namespace minni

#endif // MINNI_CORE_OPTIMIZATION_QUANTIZER_H_

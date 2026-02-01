#ifndef MINNI_CORE_SIGNAL_DSP_KERNEL_H_
#define MINNI_CORE_SIGNAL_DSP_KERNEL_H_

#include <cstddef>
#include <cstdint>

namespace minni {
namespace signal {

/**
 * Interface for low-level DSP primitives.
 * Implementations should use NEON intrinsics on ARM and AVX on x86.
 */
class DSPKernel {
public:
    /**
     * Compute the magnitude of a complex vector.
     * output[i] = sqrt(real[i]^2 + imag[i]^2)
     */
    static void complex_magnitude(const float* real, const float* imag, float* output, size_t size);

    /**
     * Apply a window function (e.g., Hanning) to an input buffer.
     * output[i] = input[i] * window[i]
     */
    static void apply_window(const float* input, const float* window, float* output, size_t size);

    /**
     * FIR (Finite Impulse Response) Filter.
     * Standard convolution.
     */
    static void fir_filter(const float* input, size_t input_size,
                           const float* taps, size_t taps_size,
                           float* output);

    /**
     * Vector addition: out = a + b
     */
    static void vector_add(const float* a, const float* b, float* out, size_t size);

    /**
     * Vector multiplication (element-wise): out = a * b
     */
    static void vector_mul(const float* a, const float* b, float* out, size_t size);

    /**
     * Dot Product: sum(a[i] * b[i])
     */
    static float dot_product(const float* a, const float* b, size_t size);

    /**
     * Cosine Similarity: dot(a, b) / (norm(a) * norm(b))
     */
    static float cosine_similarity(const float* a, const float* b, size_t size);

    /**
     * Int8 Dot Product: sum(a[i] * b[i])
     * Useful for quantized inference.
     * Returns int32_t accumulator.
     */
    static int32_t dot_product_i8(const int8_t* a, const int8_t* b, size_t size);

    /**
     * In-place Radix-2 FFT (Fast Fourier Transform).
     * Size must be a power of 2.
     * Real and imaginary parts are stored in separate arrays (split-complex format).
     *
     * @param real Real components (input and output)
     * @param imag Imaginary components (input and output)
     * @param size Number of points (must be power of 2)
     * @param inverse If true, perform Inverse FFT (IFFT)
     */
    static void fft(float* real, float* imag, size_t size, bool inverse = false);

private:
    // Helper for bit-reversal permutation
    static void bit_reverse_copy(const float* src_r, const float* src_i,
                                 float* dst_r, float* dst_i, size_t size);
};

} // namespace signal
} // namespace minni

#endif // MINNI_CORE_SIGNAL_DSP_KERNEL_H_

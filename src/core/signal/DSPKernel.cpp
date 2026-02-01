#include "DSPKernel.h"
#include <cmath>
#include <algorithm> // for std::swap

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Check for NEON support
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
    #include <arm_neon.h>
    #define HAS_NEON
#endif

namespace minni {
namespace signal {

void DSPKernel::complex_magnitude(const float* real, const float* imag, float* output, size_t size) {
    size_t i = 0;

#ifdef HAS_NEON
    // Process 4 floats at a time
    for (; i + 3 < size; i += 4) {
        float32x4_t r_vec = vld1q_f32(real + i);
        float32x4_t i_vec = vld1q_f32(imag + i);

        // r^2
        float32x4_t r_sq = vmulq_f32(r_vec, r_vec);
        // i^2
        float32x4_t i_sq = vmulq_f32(i_vec, i_vec);
        // sum = r^2 + i^2
        float32x4_t sum = vaddq_f32(r_sq, i_sq);

        // sqrt(sum) - requires ARMv8 or specific NEON capability
        // For broad compatibility, we might use an approximation or scalar loop if unavailable.
        // On modern Android (ARMv8), vsqrtq_f32 exists.
        float32x4_t result = vsqrtq_f32(sum);

        vst1q_f32(output + i, result);
    }
#endif

    // Scalar fallback (tail handling)
    for (; i < size; ++i) {
        output[i] = std::sqrt(real[i] * real[i] + imag[i] * imag[i]);
    }
}

void DSPKernel::apply_window(const float* input, const float* window, float* output, size_t size) {
    size_t i = 0;
#ifdef HAS_NEON
    for (; i + 3 < size; i += 4) {
        float32x4_t in_vec = vld1q_f32(input + i);
        float32x4_t win_vec = vld1q_f32(window + i);
        float32x4_t out_vec = vmulq_f32(in_vec, win_vec);
        vst1q_f32(output + i, out_vec);
    }
#endif
    for (; i < size; ++i) {
        output[i] = input[i] * window[i];
    }
}

void DSPKernel::fir_filter(const float* input, size_t input_size,
                           const float* taps, size_t taps_size,
                           float* output) {
    // Naive convolution: O(N*M)
    // Optimized versions would use FFT for large kernels.
    for (size_t i = 0; i < input_size; ++i) {
        float acc = 0.0f;
        for (size_t j = 0; j < taps_size; ++j) {
            if (i >= j) {
                acc += input[i - j] * taps[j];
            }
        }
        output[i] = acc;
    }
}

void DSPKernel::vector_add(const float* a, const float* b, float* out, size_t size) {
    size_t i = 0;
#ifdef HAS_NEON
    for (; i + 3 < size; i += 4) {
        float32x4_t a_vec = vld1q_f32(a + i);
        float32x4_t b_vec = vld1q_f32(b + i);
        float32x4_t res_vec = vaddq_f32(a_vec, b_vec);
        vst1q_f32(out + i, res_vec);
    }
#endif
    for (; i < size; ++i) {
        out[i] = a[i] + b[i];
    }
}

void DSPKernel::vector_mul(const float* a, const float* b, float* out, size_t size) {
    size_t i = 0;
#ifdef HAS_NEON
    for (; i + 3 < size; i += 4) {
        float32x4_t a_vec = vld1q_f32(a + i);
        float32x4_t b_vec = vld1q_f32(b + i);
        float32x4_t res_vec = vmulq_f32(a_vec, b_vec);
        vst1q_f32(out + i, res_vec);
    }
#endif
    for (; i < size; ++i) {
        out[i] = a[i] * b[i];
    }
}

float DSPKernel::dot_product(const float* a, const float* b, size_t size) {
    float sum = 0.0f;
    size_t i = 0;
#ifdef HAS_NEON
    float32x4_t sum_vec = vdupq_n_f32(0.0f);
    for (; i + 3 < size; i += 4) {
        float32x4_t a_vec = vld1q_f32(a + i);
        float32x4_t b_vec = vld1q_f32(b + i);
        sum_vec = vmlaq_f32(sum_vec, a_vec, b_vec);
    }
    // Horizontal add
    float temp[4];
    vst1q_f32(temp, sum_vec);
    sum += temp[0] + temp[1] + temp[2] + temp[3];
#endif
    for (; i < size; ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}

float DSPKernel::cosine_similarity(const float* a, const float* b, size_t size) {
    float dot = dot_product(a, b, size);
    float norm_a = std::sqrt(dot_product(a, a, size));
    float norm_b = std::sqrt(dot_product(b, b, size));

    if (norm_a < 1e-9 || norm_b < 1e-9) return 0.0f;
    return dot / (norm_a * norm_b);
}

int32_t DSPKernel::dot_product_i8(const int8_t* a, const int8_t* b, size_t size) {
    int32_t sum = 0;
    size_t i = 0;

#ifdef HAS_NEON
    int32x4_t sum_vec = vdupq_n_s32(0);
    for (; i + 15 < size; i += 16) {
        int8x16_t a_vec = vld1q_s8(a + i);
        int8x16_t b_vec = vld1q_s8(b + i);

        // Multiply and accumulate into 16-bit
        // vmull_s8 -> 8x8 = 16 (low and high)
        int16x8_t mul_low = vmull_s8(vget_low_s8(a_vec), vget_low_s8(b_vec));
        int16x8_t mul_high = vmull_s8(vget_high_s8(a_vec), vget_high_s8(b_vec));

        // Accumulate into 32-bit
        // vpadalq_s16: pairwise add and accumulate
        sum_vec = vpadalq_s16(sum_vec, mul_low);
        sum_vec = vpadalq_s16(sum_vec, mul_high);
    }
    // Reduce sum_vec
    int32_t temp[4];
    vst1q_s32(temp, sum_vec);
    sum += temp[0] + temp[1] + temp[2] + temp[3];
#endif

    for (; i < size; ++i) {
        sum += static_cast<int32_t>(a[i]) * static_cast<int32_t>(b[i]);
    }
    return sum;
}

void DSPKernel::bit_reverse_copy(const float* src_r, const float* src_i,
                                 float* dst_r, float* dst_i, size_t size) {
    int bits = 0;
    while ((1 << bits) < size) bits++;

    for (size_t i = 0; i < size; ++i) {
        size_t rev = 0;
        size_t val = i;
        for (int b = 0; b < bits; ++b) {
            rev = (rev << 1) | (val & 1);
            val >>= 1;
        }
        if (rev < size) {
            dst_r[rev] = src_r[i];
            dst_i[rev] = src_i[i];
        }
    }
}

void DSPKernel::fft(float* real, float* imag, size_t size, bool inverse) {
    // 1. Bit-reversal Permutation (In-place)
    int bits = 0;
    while ((1 << bits) < size) bits++;

    for (size_t i = 0; i < size; ++i) {
        size_t rev = 0;
        size_t val = i;
        for (int b = 0; b < bits; ++b) {
            rev = (rev << 1) | (val & 1);
            val >>= 1;
        }
        if (i < rev) {
            std::swap(real[i], real[rev]);
            std::swap(imag[i], imag[rev]);
        }
    }

    // 2. Butterfly Operations (Cooley-Tukey)
    for (size_t len = 2; len <= size; len <<= 1) {
        float ang = 2 * M_PI / len * (inverse ? -1 : 1);
        float wlen_r = std::cos(ang);
        float wlen_i = std::sin(ang);

        for (size_t i = 0; i < size; i += len) {
            float w_r = 1.0f;
            float w_i = 0.0f;
            for (size_t j = 0; j < len / 2; ++j) {
                float u_r = real[i + j];
                float u_i = imag[i + j];

                // v = w * input[i+j+len/2]
                float v_r = real[i + j + len / 2] * w_r - imag[i + j + len / 2] * w_i;
                float v_i = real[i + j + len / 2] * w_i + imag[i + j + len / 2] * w_r;

                real[i + j] = u_r + v_r;
                imag[i + j] = u_i + v_i;
                real[i + j + len / 2] = u_r - v_r;
                imag[i + j + len / 2] = u_i - v_i;

                float temp_w_r = w_r * wlen_r - w_i * wlen_i;
                w_i = w_r * wlen_i + w_i * wlen_r;
                w_r = temp_w_r;
            }
        }
    }

    // 3. Scaling for Inverse FFT
    if (inverse) {
        for (size_t i = 0; i < size; ++i) {
            real[i] /= size;
            imag[i] /= size;
        }
    }
}

} // namespace signal
} // namespace minni

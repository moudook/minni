#include "SignalProcessor.h"
#include "DSPKernel.h"
#include <algorithm>

namespace minni {
namespace signal {

SignalProcessor::SignalProcessor() = default;
SignalProcessor::~SignalProcessor() = default;

void SignalProcessor::load(const std::vector<float>& data) {
    real_ = data;
    imag_.assign(data.size(), 0.0f);
}

void SignalProcessor::apply_window(const std::vector<float>& window) {
    if (window.size() != real_.size()) return;

    // Use DSPKernel for optimized processing
    // Since DSPKernel writes to an output buffer, we can do in-place if supported
    // But our DSPKernel signature is (in, win, out).
    // Let's create a temporary or modify DSPKernel to allow in-place?
    // DSPKernel::apply_window is safe for in-place if in == out.

    DSPKernel::apply_window(real_.data(), window.data(), real_.data(), real_.size());
}

void SignalProcessor::fft(bool inverse) {
    if (real_.empty()) return;

    // Ensure size is power of 2
    size_t n = real_.size();
    if ((n & (n - 1)) != 0) return; // Not power of 2

    DSPKernel::fft(real_.data(), imag_.data(), n, inverse);
}

void SignalProcessor::magnitude() {
    if (real_.empty()) return;

    // Writes result to real_, effectively destroying the complex info
    // But DSPKernel::complex_magnitude takes (real, imag, out)
    // Overlapping buffers might be risky depending on implementation,
    // but element-wise magnitude is usually safe if loop order is correct.
    // However, let's use a temp buffer or trust the kernel.
    // Actually, our kernel implementation loops i=0..size. If out==real,
    // output[i] depends on real[i] and imag[i]. Once output[i] is written,
    // real[i] is overwritten. Since we don't look ahead, this IS safe.

    DSPKernel::complex_magnitude(real_.data(), imag_.data(), real_.data(), real_.size());

    // Clear imaginary part as it's no longer valid/needed
    std::fill(imag_.begin(), imag_.end(), 0.0f);
}

std::vector<float> SignalProcessor::get_output() const {
    return real_;
}

size_t SignalProcessor::size() const {
    return real_.size();
}

} // namespace signal
} // namespace minni

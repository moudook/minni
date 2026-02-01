#ifndef MINNI_CORE_SIGNAL_SIGNAL_PROCESSOR_H_
#define MINNI_CORE_SIGNAL_SIGNAL_PROCESSOR_H_

#include <vector>
#include <cstddef>

namespace minni {
namespace signal {

/**
 * High-level processor for chaining DSP operations.
 * Maintains state (real/imaginary buffers) to avoid repeated allocations.
 */
class SignalProcessor {
public:
    SignalProcessor();
    ~SignalProcessor();

    // Load data into the real buffer, clears imaginary buffer
    void load(const std::vector<float>& data);

    // Apply window function (e.g., Hanning) to current real buffer
    void apply_window(const std::vector<float>& window);

    // Perform in-place FFT on current buffers
    void fft(bool inverse = false);

    // Compute magnitude and store in real buffer (imag becomes 0)
    void magnitude();

    // Get the current real buffer (output)
    std::vector<float> get_output() const;

    // Get current size
    size_t size() const;

private:
    std::vector<float> real_;
    std::vector<float> imag_;
};

} // namespace signal
} // namespace minni

#endif // MINNI_CORE_SIGNAL_SIGNAL_PROCESSOR_H_

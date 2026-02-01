#ifndef MINNI_CORE_LOGIC_FLAT_VECTOR_STORE_H_
#define MINNI_CORE_LOGIC_FLAT_VECTOR_STORE_H_

#include <string>
#include <vector>
#include <utility>
#include "../platform/MemoryMapper.h"
#include "../optimization/Quantizer.h"

namespace minni {
namespace logic {

/**
 * A read-only, zero-copy Vector Store backed by a memory-mapped file.
 * Designed for extreme memory efficiency on Android (avoids LMK).
 * Reads the "MFVS" format created by VectorStore::save_flat().
 */
class FlatVectorStore {
public:
    FlatVectorStore();
    ~FlatVectorStore();

    /**
     * Load a "Flat" binary file.
     * @param path File path.
     * @return true if successful.
     */
    bool load(const std::string& path);

    /**
     * Search for nearest neighbors using zero-copy access.
     * @param query The query vector.
     * @param limit Maximum number of results.
     * @return List of (ID, Score) pairs.
     */
    std::vector<std::pair<std::string, float>> search(const std::vector<float>& query, size_t limit);

    size_t size() const;
    void close();

private:
    minni::platform::MemoryMapper mapper_;

    // Metadata from header
    size_t num_vectors_ = 0;
    size_t dim_ = 0;
    bool is_quantized_ = false;

    // Pointers into mapped memory (valid as long as mapper_ is mapped)
    const void* vectors_ptr_ = nullptr;        // Points to start of vector data
    const void* quant_params_ptr_ = nullptr;   // Points to start of params (if quantized)
    const uint64_t* id_offsets_ptr_ = nullptr; // Points to start of ID offset table
};

} // namespace logic
} // namespace minni

#endif // MINNI_CORE_LOGIC_FLAT_VECTOR_STORE_H_

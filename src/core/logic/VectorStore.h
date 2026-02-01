#ifndef MINNI_CORE_LOGIC_VECTOR_STORE_H_
#define MINNI_CORE_LOGIC_VECTOR_STORE_H_

#include <string>
#include <vector>
#include <map>
#include <utility>
#include "../optimization/Quantizer.h"

namespace minni {
namespace logic {

/**
 * A simple in-memory Vector Store for embedding retrieval.
 * Uses brute-force search (suitable for small on-device datasets).
 * Relies on DSPKernel for optimized similarity calculations.
 * Supports optional 8-bit quantization for reduced memory usage.
 */
class VectorStore {
public:
    /**
     * @param use_quantization If true, vectors are stored as int8_t.
     */
    VectorStore(bool use_quantization = false);
    ~VectorStore();

    /**
     * Add a vector to the store.
     * @param id Unique identifier for the vector.
     * @param vector The embedding data (float).
     * @return true if added, false if ID exists or empty vector.
     */
    bool add_vector(const std::string& id, const std::vector<float>& vector);

    /**
     * Search for the nearest neighbors to the query vector.
     * @param query The query vector.
     * @param limit Maximum number of results to return.
     * @return List of (ID, Score) pairs, sorted by score (descending).
     */
    std::vector<std::pair<std::string, float>> search(const std::vector<float>& query, size_t limit);

    /**
     * Get the number of vectors in the store.
     */
    size_t size() const;

    /**
     * Clear all vectors.
     */
    void clear();

    /**
     * Save the store to a binary file.
     * @param path File path.
     * @param encryption_key Optional key for encryption.
     * @return true if successful.
     */
    bool save(const std::string& path, const std::string& encryption_key = "") const;

    /**
     * Load the store from a binary file.
     * @param path File path.
     * @param encryption_key Optional key for decryption.
     * @return true if successful.
     */
    bool load(const std::string& path, const std::string& encryption_key = "");

    /**
     * Save the store to a "Flat" binary format optimized for mmap (Zero-Copy).
     * Layout: Header | Metadata | Contiguous Vector Data | ID Data
     * @param path File path.
     * @return true if successful.
     */
    bool save_flat(const std::string& path) const;

private:
    bool use_quantization_;
    size_t vector_dim_ = 0;

    // Float32 Storage
    std::map<std::string, std::vector<float>> store_;

    // Int8 Storage
    std::map<std::string, std::vector<int8_t>> quantized_store_;
    std::map<std::string, minni::optimization::Quantizer::QuantizationParams> quant_params_;

    // Helper for loading from a stream
    bool loadFromStream(std::istream& in);
};


} // namespace logic
} // namespace minni

#endif // MINNI_CORE_LOGIC_VECTOR_STORE_H_

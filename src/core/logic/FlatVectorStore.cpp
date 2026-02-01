#include "FlatVectorStore.h"
#include "../signal/DSPKernel.h"
#include <algorithm>
#include <iostream>
#include <cstring>

namespace minni {
namespace logic {

// Must match VectorStore.cpp
const char FLAT_MAGIC_HEADER[] = "MFVS";

FlatVectorStore::FlatVectorStore() = default;
FlatVectorStore::~FlatVectorStore() {
    close();
}

void FlatVectorStore::close() {
    mapper_.unmap();
    vectors_ptr_ = nullptr;
    quant_params_ptr_ = nullptr;
    id_offsets_ptr_ = nullptr;
    num_vectors_ = 0;
    dim_ = 0;
}

bool FlatVectorStore::load(const std::string& path) {
    if (!mapper_.map(path)) {
        return false;
    }

    const uint8_t* data = static_cast<const uint8_t*>(mapper_.data());
    size_t size = mapper_.size();

    if (size < 64) { // Minimum header size
        close();
        return false;
    }

    // 1. Check Header
    if (std::memcmp(data, FLAT_MAGIC_HEADER, 4) != 0) {
        close();
        return false;
    }

    // Read Metadata
    uint32_t version = *reinterpret_cast<const uint32_t*>(data + 4);
    if (version != 1) { // Version mismatch
        close();
        return false;
    }

    dim_ = static_cast<size_t>(*reinterpret_cast<const uint32_t*>(data + 8));
    uint32_t flags = *reinterpret_cast<const uint32_t*>(data + 12);
    is_quantized_ = (flags & 1);

    num_vectors_ = static_cast<size_t>(*reinterpret_cast<const uint64_t*>(data + 16));
    uint64_t vec_offset = *reinterpret_cast<const uint64_t*>(data + 24);
    uint64_t params_offset = *reinterpret_cast<const uint64_t*>(data + 32);
    uint64_t id_offset = *reinterpret_cast<const uint64_t*>(data + 40);

    // Bounds checking
    if (vec_offset >= size || id_offset >= size) {
        close();
        return false;
    }

    // Set pointers
    vectors_ptr_ = data + vec_offset;
    id_offsets_ptr_ = reinterpret_cast<const uint64_t*>(data + id_offset);

    if (is_quantized_) {
        if (params_offset >= size) {
            close();
            return false;
        }
        quant_params_ptr_ = data + params_offset;
    }

    return true;
}

std::vector<std::pair<std::string, float>> FlatVectorStore::search(const std::vector<float>& query, size_t limit) {
    std::vector<std::pair<std::string, float>> results;

    if (!mapper_.is_mapped() || num_vectors_ == 0 || query.size() != dim_) {
        return results;
    }

    // Pre-allocate results with indices temporarily to avoid string lookups during search loop
    // Store as (index, score) pairs
    using IndexScore = std::pair<size_t, float>;
    std::vector<IndexScore> candidates;
    candidates.reserve(num_vectors_);

    if (is_quantized_) {
        const int8_t* vecs = static_cast<const int8_t*>(vectors_ptr_);
        const auto* params = static_cast<const minni::optimization::Quantizer::QuantizationParams*>(quant_params_ptr_);

        for (size_t i = 0; i < num_vectors_; ++i) {
            // Locate vector i
            const int8_t* vec_i = vecs + (i * dim_);

            // Dequantize on fly (optimization opportunity: SIMD dequantize-and-dot)
            std::vector<float> dq_vec = minni::optimization::Quantizer::dequantize(
                std::vector<int8_t>(vec_i, vec_i + dim_),
                params[i]
            );

            float score = minni::signal::DSPKernel::cosine_similarity(
                query.data(), dq_vec.data(), dim_
            );
            candidates.emplace_back(i, score);
        }
    } else {
        const float* vecs = static_cast<const float*>(vectors_ptr_);

        for (size_t i = 0; i < num_vectors_; ++i) {
            const float* vec_i = vecs + (i * dim_);
            float score = minni::signal::DSPKernel::cosine_similarity(
                query.data(), vec_i, dim_
            );
            candidates.emplace_back(i, score);
        }
    }

    // Top-K selection
    // Partial sort is faster than full sort for small K
    if (limit < candidates.size()) {
        std::partial_sort(candidates.begin(), candidates.begin() + limit, candidates.end(),
            [](const IndexScore& a, const IndexScore& b) {
                return a.second > b.second; // Descending
            });
        candidates.resize(limit);
    } else {
        std::sort(candidates.begin(), candidates.end(),
            [](const IndexScore& a, const IndexScore& b) {
                return a.second > b.second;
            });
    }

    // Resolve IDs
    // The ID block starts with the offset table (which we have in id_offsets_ptr_)
    // The strings follow immediately after the table.
    // The offset table contains offsets relative to the START of the ID section (id_offset).
    // Wait, in save_flat, I wrote:
    // uint64_t current_str_relative_offset = count * 8;
    // So offsets are relative to the start of the ID block.

    const uint8_t* id_base_ptr = reinterpret_cast<const uint8_t*>(id_offsets_ptr_);

    for (const auto& candidate : candidates) {
        size_t idx = candidate.first;
        float score = candidate.second;

        uint64_t offset = id_offsets_ptr_[idx];
        const char* str_ptr = reinterpret_cast<const char*>(id_base_ptr + offset);

        results.emplace_back(std::string(str_ptr), score);
    }

    return results;
}

size_t FlatVectorStore::size() const {
    return num_vectors_;
}

} // namespace logic
} // namespace minni

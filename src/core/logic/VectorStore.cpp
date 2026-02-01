#include "VectorStore.h"
#include "../signal/DSPKernel.h"
#include "../security/SecurityManager.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

namespace minni {
namespace logic {

// File Format Constants
const char MAGIC_HEADER[] = "MVS1"; // Minni Vector Store v1
const char MAGIC_HEADER_ENC[] = "MVE1"; // Minni Vector Store Encrypted v1
const uint8_t FLAG_QUANTIZED = 0x01;

VectorStore::VectorStore(bool use_quantization)
    : use_quantization_(use_quantization) {}

VectorStore::~VectorStore() = default;

bool VectorStore::add_vector(const std::string& id, const std::vector<float>& vector) {
    if (vector.empty()) return false;

    // Check if ID exists in either store
    if (store_.count(id) > 0 || quantized_store_.count(id) > 0) return false;

    // Check dimensionality consistency
    size_t current_dim = vector.size();
    if (vector_dim_ == 0) {
        vector_dim_ = current_dim;
    } else if (current_dim != vector_dim_) {
        return false;
    }

    if (use_quantization_) {
        // Calculate params and quantize
        auto params = minni::optimization::Quantizer::calculate_params(vector);
        auto quantized = minni::optimization::Quantizer::quantize(vector, params);

        quantized_store_[id] = quantized;
        quant_params_[id] = params;
    } else {
        store_[id] = vector;
    }

    return true;
}

std::vector<std::pair<std::string, float>> VectorStore::search(const std::vector<float>& query, size_t limit) {
    std::vector<std::pair<std::string, float>> results;

    if (query.size() != vector_dim_ || (store_.empty() && quantized_store_.empty())) {
        return results;
    }

    if (use_quantization_) {
        results.reserve(quantized_store_.size());
        for (const auto& kv : quantized_store_) {
            const std::string& id = kv.first;
            const std::vector<int8_t>& q_vec = kv.second;
            const auto& params = quant_params_.at(id);

            // Dequantize on the fly for similarity calculation
            // (Optimization: In future, implement direct quantized cosine similarity)
            std::vector<float> vec = minni::optimization::Quantizer::dequantize(q_vec, params);

            float score = minni::signal::DSPKernel::cosine_similarity(
                query.data(), vec.data(), vector_dim_
            );
            results.emplace_back(id, score);
        }
    } else {
        results.reserve(store_.size());
        for (const auto& kv : store_) {
            const std::string& id = kv.first;
            const std::vector<float>& vec = kv.second;

            float score = minni::signal::DSPKernel::cosine_similarity(
                query.data(), vec.data(), vector_dim_
            );
            results.emplace_back(id, score);
        }
    }

    // Sort by score descending (highest similarity first)
    std::sort(results.begin(), results.end(),
              [](const std::pair<std::string, float>& a, const std::pair<std::string, float>& b) {
                  return a.second > b.second;
              });

    // Truncate to limit
    if (results.size() > limit) {
        results.resize(limit);
    }

    return results;
}

size_t VectorStore::size() const {
    return use_quantization_ ? quantized_store_.size() : store_.size();
}

void VectorStore::clear() {
    store_.clear();
    quantized_store_.clear();
    quant_params_.clear();
    vector_dim_ = 0;
}

bool VectorStore::save(const std::string& path, const std::string& encryption_key) const {
    // 1. Serialize data to a buffer first (needed for encryption anyway)
    // Or write directly to file if no encryption.

    std::stringstream ss;

    // Header for internal data (MVS1)
    ss.write(MAGIC_HEADER, 4);

    // Flags
    uint8_t flags = 0;
    if (use_quantization_) flags |= FLAG_QUANTIZED;
    ss.write(reinterpret_cast<const char*>(&flags), sizeof(flags));

    // Metadata
    uint32_t dim = static_cast<uint32_t>(vector_dim_);
    ss.write(reinterpret_cast<const char*>(&dim), sizeof(dim));

    size_t count = size();
    uint64_t num_vecs = static_cast<uint64_t>(count);
    ss.write(reinterpret_cast<const char*>(&num_vecs), sizeof(num_vecs));

    // Data
    if (use_quantization_) {
        for (const auto& kv : quantized_store_) {
            const std::string& id = kv.first;
            const std::vector<int8_t>& vec = kv.second;
            const auto& params = quant_params_.at(id);

            // ID
            uint32_t id_len = static_cast<uint32_t>(id.size());
            ss.write(reinterpret_cast<const char*>(&id_len), sizeof(id_len));
            ss.write(id.data(), id_len);

            // Quant Params
            ss.write(reinterpret_cast<const char*>(&params.scale), sizeof(params.scale));
            ss.write(reinterpret_cast<const char*>(&params.zero_point), sizeof(params.zero_point));

            // Vector Data
            ss.write(reinterpret_cast<const char*>(vec.data()), vec.size() * sizeof(int8_t));
        }
    } else {
        for (const auto& kv : store_) {
            const std::string& id = kv.first;
            const std::vector<float>& vec = kv.second;

            // ID
            uint32_t id_len = static_cast<uint32_t>(id.size());
            ss.write(reinterpret_cast<const char*>(&id_len), sizeof(id_len));
            ss.write(id.data(), id_len);

            // Vector Data
            ss.write(reinterpret_cast<const char*>(vec.data()), vec.size() * sizeof(float));
        }
    }

    // 2. Handle Encryption or Direct Write
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;

    std::string serialized_data = ss.str();

    if (!encryption_key.empty()) {
        out.write(MAGIC_HEADER_ENC, 4);

        // Encrypt
        std::vector<uint8_t> raw_data(serialized_data.begin(), serialized_data.end());
        std::vector<uint8_t> encrypted = minni::security::SecurityManager::encrypt(raw_data, encryption_key);

        // Write size of encrypted blob (for safety/validation, though mostly EOF works)
        // uint64_t enc_size = encrypted.size();
        // out.write(reinterpret_cast<const char*>(&enc_size), sizeof(enc_size));

        out.write(reinterpret_cast<const char*>(encrypted.data()), encrypted.size());
    } else {
        // Just write the serialized data buffer directly
        // Note: The buffer already contains MAGIC_HEADER at the start
        out.write(serialized_data.data(), serialized_data.size());
    }

    out.close();
    return out.good();
}

bool VectorStore::load(const std::string& path, const std::string& encryption_key) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return false;

    // Check header
    char header[5] = {0};
    in.read(header, 4);

    std::string header_str(header);

    if (header_str == MAGIC_HEADER_ENC) {
        if (encryption_key.empty()) {
            std::cerr << "Error: File is encrypted but no key provided." << std::endl;
            return false;
        }

        // Read remaining data
        std::vector<uint8_t> encrypted_data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

        // Decrypt
        std::vector<uint8_t> decrypted_data = minni::security::SecurityManager::decrypt(encrypted_data, encryption_key);

        // Create stream from decrypted data
        std::string decrypted_string(decrypted_data.begin(), decrypted_data.end());
        std::stringstream ss(decrypted_string);

        // Verify inner header
        char inner_header[5] = {0};
        ss.read(inner_header, 4);
        if (std::string(inner_header) != MAGIC_HEADER) {
            return false;
        }

        return loadFromStream(ss);

    } else if (header_str == MAGIC_HEADER) {
        // Standard unencrypted file
        return loadFromStream(in);
    } else {
        return false;
    }
}

bool VectorStore::loadFromStream(std::istream& in) {
    // 2. Flags
    uint8_t flags = 0;
    in.read(reinterpret_cast<char*>(&flags), sizeof(flags));
    bool is_quantized = (flags & FLAG_QUANTIZED);

    // Ensure current store configuration matches file
    if (is_quantized != use_quantization_) {
        clear();
        use_quantization_ = is_quantized;
    } else {
        clear();
    }

    // 3. Metadata
    uint32_t dim = 0;
    in.read(reinterpret_cast<char*>(&dim), sizeof(dim));
    vector_dim_ = dim;

    uint64_t num_vecs = 0;
    in.read(reinterpret_cast<char*>(&num_vecs), sizeof(num_vecs));

    // 4. Data
    for (uint64_t i = 0; i < num_vecs; ++i) {
        // ID
        uint32_t id_len = 0;
        in.read(reinterpret_cast<char*>(&id_len), sizeof(id_len));

        std::string id(id_len, '\0');
        in.read(&id[0], id_len);

        if (use_quantization_) {
            minni::optimization::Quantizer::QuantizationParams params;
            in.read(reinterpret_cast<char*>(&params.scale), sizeof(params.scale));
            in.read(reinterpret_cast<char*>(&params.zero_point), sizeof(params.zero_point));

            std::vector<int8_t> vec(vector_dim_);
            in.read(reinterpret_cast<char*>(vec.data()), vector_dim_ * sizeof(int8_t));

            quantized_store_[id] = vec;
            quant_params_[id] = params;
        } else {
            std::vector<float> vec(vector_dim_);
            in.read(reinterpret_cast<char*>(vec.data()), vector_dim_ * sizeof(float));

            store_[id] = vec;
        }
    }

    return in.good();
}

const char FLAT_MAGIC_HEADER[] = "MFVS"; // Minni Flat Vector Store v1

bool VectorStore::save_flat(const std::string& path) const {
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;

    // Header Structure (Fixed 64 bytes for simplicity/alignment)
    // 0-3: Magic "MFVS"
    // 4-7: Version (1)
    // 8-11: Dim
    // 12-15: Flags (bit 0 = quantized)
    // 16-23: NumVectors
    // 24-31: Vector Data Offset
    // 32-39: Quant Params Offset (or 0)
    // 40-47: ID Blob Offset
    // 48-63: Reserved

    uint32_t version = 1;
    uint32_t dim = static_cast<uint32_t>(vector_dim_);
    uint32_t flags = use_quantization_ ? 1 : 0;
    uint64_t count = size();

    // Calculate offsets
    // Header is 64 bytes
    uint64_t vec_offset = 64;
    uint64_t vec_size = 0;

    if (use_quantization_) {
        vec_size = count * dim * sizeof(int8_t);
    } else {
        vec_size = count * dim * sizeof(float);
    }

    uint64_t params_offset = 0;
    uint64_t params_size = 0;
    if (use_quantization_) {
        // align to 4 bytes
        while ((vec_offset + vec_size) % 4 != 0) vec_size++;
        params_offset = vec_offset + vec_size;
        params_size = count * sizeof(minni::optimization::Quantizer::QuantizationParams);
    }

    uint64_t id_offset = use_quantization_ ? (params_offset + params_size) : (vec_offset + vec_size);

    // Write Header
    out.write(FLAT_MAGIC_HEADER, 4);
    out.write(reinterpret_cast<const char*>(&version), 4);
    out.write(reinterpret_cast<const char*>(&dim), 4);
    out.write(reinterpret_cast<const char*>(&flags), 4);
    out.write(reinterpret_cast<const char*>(&count), 8);
    out.write(reinterpret_cast<const char*>(&vec_offset), 8);
    out.write(reinterpret_cast<const char*>(&params_offset), 8);
    out.write(reinterpret_cast<const char*>(&id_offset), 8);

    char reserved[16] = {0};
    out.write(reserved, 16);

    // Write Data
    // We iterate the map. This defines the implicit index i for each ID.

    // 1. Vector Data
    out.seekp(vec_offset);
    if (use_quantization_) {
        for (const auto& kv : quantized_store_) {
            const auto& vec = kv.second;
            out.write(reinterpret_cast<const char*>(vec.data()), dim * sizeof(int8_t));
        }
    } else {
        for (const auto& kv : store_) {
            const auto& vec = kv.second;
            out.write(reinterpret_cast<const char*>(vec.data()), dim * sizeof(float));
        }
    }

    // 2. Quant Params (if needed)
    if (use_quantization_) {
        out.seekp(params_offset);
        for (const auto& kv : quantized_store_) {
            const std::string& id = kv.first;
            const auto& params = quant_params_.at(id);
            out.write(reinterpret_cast<const char*>(&params), sizeof(params));
        }
    }

    // 3. ID Blob
    out.seekp(id_offset);

    // Calculate total size of ID strings to determine offsets
    std::vector<uint64_t> str_offsets;
    str_offsets.reserve(count);

    uint64_t current_str_relative_offset = count * 8; // offsets start after the table

    // Pass 1: Calculate offsets
    if (use_quantization_) {
        for (const auto& kv : quantized_store_) {
            str_offsets.push_back(current_str_relative_offset);
            current_str_relative_offset += kv.first.size() + 1; // +1 for null terminator
        }
    } else {
        for (const auto& kv : store_) {
            str_offsets.push_back(current_str_relative_offset);
            current_str_relative_offset += kv.first.size() + 1;
        }
    }

    // Write Offset Table
    out.write(reinterpret_cast<const char*>(str_offsets.data()), str_offsets.size() * 8);

    // Write Strings
    if (use_quantization_) {
        for (const auto& kv : quantized_store_) {
            out.write(kv.first.c_str(), kv.first.size() + 1);
        }
    } else {
        for (const auto& kv : store_) {
            out.write(kv.first.c_str(), kv.first.size() + 1);
        }
    }

    out.close();
    return out.good();
}


} // namespace logic
} // namespace minni

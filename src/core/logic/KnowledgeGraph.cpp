#include "KnowledgeGraph.h"
#include "../signal/DSPKernel.h"
#include "../security/SecurityManager.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace minni {
namespace logic {

const char KG_MAGIC_HEADER[] = "MKG1"; // Minni Knowledge Graph v1
const char KG_MAGIC_HEADER_ENC[] = "MKGE"; // Minni Knowledge Graph Encrypted
const uint8_t KG_FLAG_QUANTIZED = 0x01;

KnowledgeGraph::KnowledgeGraph(bool use_quantization)
    : use_quantization_(use_quantization) {}

KnowledgeGraph::~KnowledgeGraph() = default;

EntityId KnowledgeGraph::add_entity(const std::string& name) {
    auto it = entity_map_.find(name);
    if (it != entity_map_.end()) {
        return it->second;
    }

    EntityId id = static_cast<EntityId>(entity_names_.size());
    entity_names_.push_back(name);
    entity_map_[name] = id;

    // Resize adjacency list to accommodate new entity
    // We treat 'id' as the index in the adj_list_ vector
    if (adj_list_.size() <= id) {
        adj_list_.resize(id + 1);
    }

    return id;
}

std::string KnowledgeGraph::get_entity_name(EntityId id) const {
    if (id < entity_names_.size()) {
        return entity_names_[id];
    }
    return "";
}

bool KnowledgeGraph::has_entity(const std::string& name) const {
    return entity_map_.find(name) != entity_map_.end();
}

RelationId KnowledgeGraph::add_relation(const std::string& name) {
    auto it = relation_map_.find(name);
    if (it != relation_map_.end()) {
        return it->second;
    }

    RelationId id = static_cast<RelationId>(relation_names_.size());
    relation_names_.push_back(name);
    relation_map_[name] = id;
    return id;
}

std::string KnowledgeGraph::get_relation_name(RelationId id) const {
    if (id < relation_names_.size()) {
        return relation_names_[id];
    }
    return "";
}

void KnowledgeGraph::add_fact(EntityId sub, RelationId pred, EntityId obj) {
    // Ensure entities exist (bounds check could be added here)
    if (sub >= adj_list_.size()) {
        adj_list_.resize(sub + 1);
    }

    // Add edge: Subject -> (Predicate, Object)
    // Check for duplicates to maintain set semantics?
    // For performance, we might skip duplicate check or do it lazily.
    // Here we do a simple check.
    auto& edges = adj_list_[sub];
    bool exists = false;
    for (const auto& edge : edges) {
        if (edge.first == pred && edge.second == obj) {
            exists = true;
            break;
        }
    }

    if (!exists) {
        edges.push_back({pred, obj});
    }
}

void KnowledgeGraph::add_fact(const std::string& sub, const std::string& pred, const std::string& obj) {
    EntityId s_id = add_entity(sub);
    RelationId p_id = add_relation(pred);
    EntityId o_id = add_entity(obj);
    add_fact(s_id, p_id, o_id);
}

std::vector<EntityId> KnowledgeGraph::query_objects(EntityId sub, RelationId pred) const {
    std::vector<EntityId> results;
    if (sub >= adj_list_.size()) {
        return results;
    }

    const auto& edges = adj_list_[sub];
    for (const auto& edge : edges) {
        if (edge.first == pred) {
            results.push_back(edge.second);
        }
    }
    return results;
}

std::vector<EntityId> KnowledgeGraph::query_subjects(RelationId pred, EntityId obj) const {
    // This is slow (O(E)) because our adjacency list is subject-indexed.
    // A production KG would maintain a reverse index (Object -> Subject).
    std::vector<EntityId> results;

    for (EntityId sub = 0; sub < adj_list_.size(); ++sub) {
        const auto& edges = adj_list_[sub];
        for (const auto& edge : edges) {
            if (edge.first == pred && edge.second == obj) {
                results.push_back(sub);
            }
        }
    }
    return results;
}

size_t KnowledgeGraph::num_entities() const {
    return entity_names_.size();
}

size_t KnowledgeGraph::num_facts() const {
    size_t count = 0;
    for (const auto& edges : adj_list_) {
        count += edges.size();
    }
    return count;
}

size_t KnowledgeGraph::memory_usage_bytes() const {
    size_t size = 0;
    // Estimate string storage
    for (const auto& s : entity_names_) size += s.capacity();
    for (const auto& s : relation_names_) size += s.capacity();

    // Estimate vector storage
    size += entity_names_.capacity() * sizeof(std::string);
    size += relation_names_.capacity() * sizeof(std::string);
    size += adj_list_.capacity() * sizeof(std::vector<std::pair<RelationId, EntityId>>);

    for (const auto& edges : adj_list_) {
        size += edges.capacity() * sizeof(std::pair<RelationId, EntityId>);
    }

    if (use_quantization_) {
        size += entity_quantized_embeddings_.capacity() * sizeof(std::vector<int8_t>);
        size += entity_quant_params_.capacity() * sizeof(minni::optimization::Quantizer::QuantizationParams);
        for (const auto& q : entity_quantized_embeddings_) {
            size += q.capacity() * sizeof(int8_t);
        }
    } else {
        size += entity_embeddings_.capacity() * sizeof(std::vector<float>);
        for (const auto& v : entity_embeddings_) {
            size += v.capacity() * sizeof(float);
        }
    }

    // Maps overhead is harder to estimate, ignoring for now
    return size;
}

bool KnowledgeGraph::set_embedding(const std::string& entity, const std::vector<float>& vector) {
    if (!has_entity(entity)) return false;
    if (vector.empty()) return false;

    EntityId id = entity_map_.at(entity);

    // Check dimensions
    if (embedding_dim_ == 0) {
        embedding_dim_ = vector.size();
    } else if (vector.size() != embedding_dim_) {
        return false;
    }

    if (use_quantization_) {
        // Ensure storage is big enough
        if (entity_quantized_embeddings_.size() <= id) {
            entity_quantized_embeddings_.resize(id + 1);
            entity_quant_params_.resize(id + 1);
        }

        auto params = minni::optimization::Quantizer::calculate_params(vector);
        entity_quantized_embeddings_[id] = minni::optimization::Quantizer::quantize(vector, params);
        entity_quant_params_[id] = params;
    } else {
        // Ensure storage is big enough
        if (entity_embeddings_.size() <= id) {
            entity_embeddings_.resize(id + 1);
        }
        entity_embeddings_[id] = vector;
    }

    return true;
}

std::vector<float> KnowledgeGraph::get_embedding(const std::string& entity) const {
    if (!has_entity(entity)) return {};
    EntityId id = entity_map_.at(entity);

    if (use_quantization_) {
        if (id >= entity_quantized_embeddings_.size()) return {};
        const auto& q_vec = entity_quantized_embeddings_[id];
        if (q_vec.empty()) return {};
        return minni::optimization::Quantizer::dequantize(q_vec, entity_quant_params_[id]);
    } else {
        if (id >= entity_embeddings_.size()) return {};
        return entity_embeddings_[id];
    }
}

std::vector<std::pair<std::string, float>> KnowledgeGraph::find_similar_entities(const std::vector<float>& query, size_t limit) const {
    std::vector<std::pair<std::string, float>> results;

    if (query.size() != embedding_dim_ || embedding_dim_ == 0) {
        return results;
    }

    size_t num_entities = entity_names_.size();
    size_t storage_size = use_quantization_ ? entity_quantized_embeddings_.size() : entity_embeddings_.size();

    // Brute force search over all entities that have embeddings
    for (EntityId id = 0; id < storage_size; ++id) {
        float score = 0.0f;
        bool has_embedding = false;

        if (use_quantization_) {
            const auto& q_vec = entity_quantized_embeddings_[id];
            if (!q_vec.empty()) {
                has_embedding = true;
                // Dequantize on fly for similarity
                auto vec = minni::optimization::Quantizer::dequantize(q_vec, entity_quant_params_[id]);
                score = minni::signal::DSPKernel::cosine_similarity(
                    query.data(), vec.data(), embedding_dim_
                );
            }
        } else {
            const auto& vec = entity_embeddings_[id];
            if (!vec.empty()) {
                has_embedding = true;
                score = minni::signal::DSPKernel::cosine_similarity(
                    query.data(), vec.data(), embedding_dim_
                );
            }
        }

        if (has_embedding) {
            results.push_back({get_entity_name(id), score});
        }
    }

    // Sort descending
    std::sort(results.begin(), results.end(),
              [](const std::pair<std::string, float>& a, const std::pair<std::string, float>& b) {
                  return a.second > b.second;
              });

    if (results.size() > limit) {
        results.resize(limit);
    }

    return results;
}

bool KnowledgeGraph::save(const std::string& path, const std::string& encryption_key) const {
    std::stringstream ss;

    // 1. Header
    ss.write(KG_MAGIC_HEADER, 4);

    // 2. Flags
    uint8_t flags = 0;
    if (use_quantization_) flags |= KG_FLAG_QUANTIZED;
    ss.write(reinterpret_cast<const char*>(&flags), sizeof(flags));

    // 3. Entities
    uint32_t num_entities = static_cast<uint32_t>(entity_names_.size());
    ss.write(reinterpret_cast<const char*>(&num_entities), sizeof(num_entities));

    for (const auto& name : entity_names_) {
        uint32_t len = static_cast<uint32_t>(name.size());
        ss.write(reinterpret_cast<const char*>(&len), sizeof(len));
        ss.write(name.data(), len);
    }

    // 4. Relations
    uint32_t num_relations = static_cast<uint32_t>(relation_names_.size());
    ss.write(reinterpret_cast<const char*>(&num_relations), sizeof(num_relations));

    for (const auto& name : relation_names_) {
        uint32_t len = static_cast<uint32_t>(name.size());
        ss.write(reinterpret_cast<const char*>(&len), sizeof(len));
        ss.write(name.data(), len);
    }

    // 5. Adjacency List (Facts)
    // We iterate over entities (subjects)
    uint32_t adj_size = static_cast<uint32_t>(adj_list_.size());
    ss.write(reinterpret_cast<const char*>(&adj_size), sizeof(adj_size));

    for (const auto& edges : adj_list_) {
        uint32_t num_edges = static_cast<uint32_t>(edges.size());
        ss.write(reinterpret_cast<const char*>(&num_edges), sizeof(num_edges));
        for (const auto& edge : edges) {
            // RelationId (uint16), EntityId (uint32)
            ss.write(reinterpret_cast<const char*>(&edge.first), sizeof(edge.first));
            ss.write(reinterpret_cast<const char*>(&edge.second), sizeof(edge.second));
        }
    }

    // 6. Embeddings
    uint32_t dim = static_cast<uint32_t>(embedding_dim_);
    ss.write(reinterpret_cast<const char*>(&dim), sizeof(dim));

    if (use_quantization_) {
        // Quantized
        uint32_t embed_count = static_cast<uint32_t>(entity_quantized_embeddings_.size());
        ss.write(reinterpret_cast<const char*>(&embed_count), sizeof(embed_count));

        for (size_t i = 0; i < embed_count; ++i) {
            const auto& q_vec = entity_quantized_embeddings_[i];

            // Valid flag (if vector is empty/not set)
            uint8_t has_embed = !q_vec.empty();
            ss.write(reinterpret_cast<const char*>(&has_embed), sizeof(has_embed));

            if (has_embed) {
                const auto& params = entity_quant_params_[i];
                ss.write(reinterpret_cast<const char*>(&params.scale), sizeof(params.scale));
                ss.write(reinterpret_cast<const char*>(&params.zero_point), sizeof(params.zero_point));
                ss.write(reinterpret_cast<const char*>(q_vec.data()), dim * sizeof(int8_t));
            }
        }
    } else {
        // Float
        uint32_t embed_count = static_cast<uint32_t>(entity_embeddings_.size());
        ss.write(reinterpret_cast<const char*>(&embed_count), sizeof(embed_count));

        for (const auto& vec : entity_embeddings_) {
            uint8_t has_embed = !vec.empty();
            ss.write(reinterpret_cast<const char*>(&has_embed), sizeof(has_embed));

            if (has_embed) {
                ss.write(reinterpret_cast<const char*>(vec.data()), dim * sizeof(float));
            }
        }
    }

    // Write to file
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;

    std::string serialized_data = ss.str();

    if (!encryption_key.empty()) {
        out.write(KG_MAGIC_HEADER_ENC, 4);

        std::vector<uint8_t> raw_data(serialized_data.begin(), serialized_data.end());
        std::vector<uint8_t> encrypted = minni::security::SecurityManager::encrypt(raw_data, encryption_key);

        out.write(reinterpret_cast<const char*>(encrypted.data()), encrypted.size());
    } else {
        out.write(serialized_data.data(), serialized_data.size());
    }

    out.close();
    return out.good();
}

bool KnowledgeGraph::load(const std::string& path, const std::string& encryption_key) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return false;

    char header[5] = {0};
    in.read(header, 4);
    std::string header_str(header);

    if (header_str == KG_MAGIC_HEADER_ENC) {
        if (encryption_key.empty()) return false;

        std::vector<uint8_t> encrypted_data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        std::vector<uint8_t> decrypted_data = minni::security::SecurityManager::decrypt(encrypted_data, encryption_key);

        std::string decrypted_string(decrypted_data.begin(), decrypted_data.end());
        std::stringstream ss(decrypted_string);

        char inner_header[5] = {0};
        ss.read(inner_header, 4);
        if (std::string(inner_header) != KG_MAGIC_HEADER) {
            return false;
        }

        return loadFromStream(ss);

    } else if (header_str == KG_MAGIC_HEADER) {
        return loadFromStream(in);
    }

    return false;
}

bool KnowledgeGraph::loadFromStream(std::istream& in) {
    // Reset state
    entity_map_.clear();
    entity_names_.clear();
    relation_map_.clear();
    relation_names_.clear();
    adj_list_.clear();
    entity_embeddings_.clear();
    entity_quantized_embeddings_.clear();
    entity_quant_params_.clear();
    embedding_dim_ = 0;

    // 2. Flags
    uint8_t flags = 0;
    in.read(reinterpret_cast<char*>(&flags), sizeof(flags));
    bool is_quantized = (flags & KG_FLAG_QUANTIZED);

    // Enforce matching quantization mode
    if (is_quantized != use_quantization_) {
        // Switch mode to allow loading
        use_quantization_ = is_quantized;
    }

    // 3. Entities
    uint32_t num_entities = 0;
    in.read(reinterpret_cast<char*>(&num_entities), sizeof(num_entities));

    entity_names_.reserve(num_entities);
    for (uint32_t i = 0; i < num_entities; ++i) {
        uint32_t len = 0;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        std::string name(len, '\0');
        in.read(&name[0], len);

        entity_names_.push_back(name);
        entity_map_[name] = static_cast<EntityId>(i);
    }

    // 4. Relations
    uint32_t num_relations = 0;
    in.read(reinterpret_cast<char*>(&num_relations), sizeof(num_relations));

    relation_names_.reserve(num_relations);
    for (uint32_t i = 0; i < num_relations; ++i) {
        uint32_t len = 0;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        std::string name(len, '\0');
        in.read(&name[0], len);

        relation_names_.push_back(name);
        relation_map_[name] = static_cast<RelationId>(i);
    }

    // 5. Adjacency List
    uint32_t adj_size = 0;
    in.read(reinterpret_cast<char*>(&adj_size), sizeof(adj_size));
    adj_list_.resize(adj_size);

    for (uint32_t i = 0; i < adj_size; ++i) {
        uint32_t num_edges = 0;
        in.read(reinterpret_cast<char*>(&num_edges), sizeof(num_edges));

        auto& edges = adj_list_[i];
        edges.reserve(num_edges);

        for (uint32_t j = 0; j < num_edges; ++j) {
            RelationId r;
            EntityId e;
            in.read(reinterpret_cast<char*>(&r), sizeof(r));
            in.read(reinterpret_cast<char*>(&e), sizeof(e));
            edges.push_back({r, e});
        }
    }

    // 6. Embeddings
    uint32_t dim = 0;
    in.read(reinterpret_cast<char*>(&dim), sizeof(dim));
    embedding_dim_ = dim;

    uint32_t embed_count = 0;
    in.read(reinterpret_cast<char*>(&embed_count), sizeof(embed_count));

    if (use_quantization_) {
        entity_quantized_embeddings_.resize(embed_count);
        entity_quant_params_.resize(embed_count);

        for (uint32_t i = 0; i < embed_count; ++i) {
            uint8_t has_embed = 0;
            in.read(reinterpret_cast<char*>(&has_embed), sizeof(has_embed));

            if (has_embed) {
                auto& params = entity_quant_params_[i];
                in.read(reinterpret_cast<char*>(&params.scale), sizeof(params.scale));
                in.read(reinterpret_cast<char*>(&params.zero_point), sizeof(params.zero_point));

                auto& q_vec = entity_quantized_embeddings_[i];
                q_vec.resize(dim);
                in.read(reinterpret_cast<char*>(q_vec.data()), dim * sizeof(int8_t));
            }
        }
    } else {
        entity_embeddings_.resize(embed_count);
        for (uint32_t i = 0; i < embed_count; ++i) {
            uint8_t has_embed = 0;
            in.read(reinterpret_cast<char*>(&has_embed), sizeof(has_embed));

            if (has_embed) {
                auto& vec = entity_embeddings_[i];
                vec.resize(dim);
                in.read(reinterpret_cast<char*>(vec.data()), dim * sizeof(float));
            }
        }
    }

    return in.good();
}


} // namespace logic
} // namespace minni

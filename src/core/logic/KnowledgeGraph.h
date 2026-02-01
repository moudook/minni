#ifndef MINNI_CORE_LOGIC_KNOWLEDGE_GRAPH_H_
#define MINNI_CORE_LOGIC_KNOWLEDGE_GRAPH_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include "../optimization/Quantizer.h"

namespace minni {
namespace logic {

using EntityId = uint32_t;
using RelationId = uint16_t;

struct Triple {
    EntityId subject;
    RelationId predicate;
    EntityId object;
};

/**
 * A lightweight, in-memory Knowledge Graph optimized for mobile RAM.
 * Uses integer IDs for storage and separate string tables for lookup.
 * Supports optional 8-bit quantization for vector embeddings.
 */
class KnowledgeGraph {
public:
    /**
     * @param use_quantization If true, embeddings are stored as int8_t to save memory.
     */
    KnowledgeGraph(bool use_quantization = false);
    ~KnowledgeGraph();

    // Entity Management
    EntityId add_entity(const std::string& name);
    std::string get_entity_name(EntityId id) const;
    bool has_entity(const std::string& name) const;

    // Relation Management
    RelationId add_relation(const std::string& name);
    std::string get_relation_name(RelationId id) const;

    // Fact Management
    void add_fact(EntityId sub, RelationId pred, EntityId obj);
    void add_fact(const std::string& sub, const std::string& pred, const std::string& obj);

    // Querying
    std::vector<EntityId> query_objects(EntityId sub, RelationId pred) const;
    std::vector<EntityId> query_subjects(RelationId pred, EntityId obj) const;

    // Metrics
    size_t num_entities() const;
    size_t num_facts() const;
    size_t memory_usage_bytes() const;

    // Vector Embeddings (Semantic Search)
    // Returns true if entity exists and vector dimensionality matches
    bool set_embedding(const std::string& entity, const std::vector<float>& vector);

    // Get embedding for an entity (returns empty if none)
    std::vector<float> get_embedding(const std::string& entity) const;

    // Search for entities with similar embeddings
    // Returns list of (EntityName, SimilarityScore)
    std::vector<std::pair<std::string, float>> find_similar_entities(const std::vector<float>& query, size_t limit) const;

    /**
     * Save the graph to a binary file.
     * @param path File path.
     * @param encryption_key Optional key for encryption.
     * @return true if successful.
     */
    bool save(const std::string& path, const std::string& encryption_key = "") const;

    /**
     * Load the graph from a binary file.
     * @param path File path.
     * @param encryption_key Optional key for decryption.
     * @return true if successful.
     */
    bool load(const std::string& path, const std::string& encryption_key = "");

private:
    // Helper for loading from a stream
    bool loadFromStream(std::istream& in);

    // String interning tables
    std::unordered_map<std::string, EntityId> entity_map_;
    std::vector<std::string> entity_names_;

    // Embeddings: index corresponds to EntityId. Empty vector if no embedding set.
    // Only one of these will be populated based on use_quantization_.
    std::vector<std::vector<float>> entity_embeddings_;
    std::vector<std::vector<int8_t>> entity_quantized_embeddings_;
    std::vector<minni::optimization::Quantizer::QuantizationParams> entity_quant_params_;

    bool use_quantization_;
    size_t embedding_dim_ = 0;

    std::unordered_map<std::string, RelationId> relation_map_;
    std::vector<std::string> relation_names_;

    // Adjacency list: subject -> [(predicate, object)]
    // Optimized for "Subject-centric" queries
    std::vector<std::vector<std::pair<RelationId, EntityId>>> adj_list_;
};

} // namespace logic
} // namespace minni

#endif // MINNI_CORE_LOGIC_KNOWLEDGE_GRAPH_H_
